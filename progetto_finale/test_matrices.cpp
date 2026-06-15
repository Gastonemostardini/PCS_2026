#include <iostream>
#include <fstream>
#include <list>
#include <set>
#include <cmath>
#include <cstdlib>
#include <stdexcept>
#include <Eigen/Dense>
#include "edge.h"
#include "graph.h"
#include "tree.h"
#include "search_graph.h"
#include "cycles.h"
#include "dfs_project.h"
#include "input.hpp"
#include "matrices.h"

using namespace std;

// Test per build_matrices (matrices.h): costruzione delle matrici del metodo
// delle maglie (B incidenza segnata rami x maglie, R resistenze, v generatori).
//
// Ogni test e' autosufficiente: scrive un file di circuito, lo fa leggere al
// parser, ricava grafo e base delle maglie (DFS, come nel main) e poi invoca
// build_matrices.
//
// Circuito A (un triangolo, una sola maglia):
//   V1 (10 V) tra 1-2,  R1 (2 ohm) tra 2-3,  R2 (3 ohm) tra 1-3
//   archi ordinati: (1,2)=V1  (1,3)=R2  (2,3)=R1
//   maglia unica con R in serie 2+3=5 ohm  ->  |I| = 10/5 = 2 A
//
// Circuito B (un triangolo di sole resistenze, nessun generatore):
//   R1,R2,R3 -> il vettore v deve risultare nullo.
class MatricesTest
{
    static constexpr const char *CIRCUIT_A = "V1 10 1 2\nR1 2 2 3\nR2 3 1 3\n";
    static constexpr const char *CIRCUIT_B = "R1 1 1 2\nR2 2 2 3\nR3 3 1 3\n";

    // scrive il file di circuito e ritorna il parser popolato e ripulito
    static input<int> parse_circuit(const string &path, const string &content)
    {
        {
            ofstream ofs(path);
            ofs << content;
        }
        input<int> parser;
        parser.parse_file(path);
        parser.clean_graph();
        return parser;
    }

    // base delle maglie tramite albero di supporto DFS (come nel main)
    static list<Cycles<int>> mesh_base(const Graph<int> &graph)
    {
        int radice = *graph.all_nodes().begin();
        return find_minimal_cycles(graph, recursive_dfs(graph, radice));
    }

public:
    // Le dimensioni devono essere B: m x k, R: m x m, v: k (m = #rami, k = #maglie).
    int test_matrix_dimensions()
    {
        try
        {
            input<int> parser = parse_circuit("test_matrices_A.txt", CIRCUIT_A);
            Graph<int> graph = parser.get_graph();
            list<Cycles<int>> base = mesh_base(graph);
            CircuitMatrices mats = build_matrices(graph, base, parser);

            int m = graph.all_edges().size();
            int k = base.size();
            if (mats.B.rows() != m || mats.B.cols() != k)
                return EXIT_FAILURE;
            if (mats.R.rows() != m || mats.R.cols() != m)
                return EXIT_FAILURE;
            if (mats.v.size() != k)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // R deve essere diagonale e contenere i valori delle resistenze
    // (0 sulla diagonale per i rami con generatore, 0 fuori diagonale).
    int test_R_is_diagonal_resistances()
    {
        try
        {
            input<int> parser = parse_circuit("test_matrices_A.txt", CIRCUIT_A);
            Graph<int> graph = parser.get_graph();
            list<Cycles<int>> base = mesh_base(graph);
            CircuitMatrices mats = build_matrices(graph, base, parser);

            for (const auto &arco : graph.all_edges())
            {
                unsigned int r = graph.edge_number(arco);
                Component<int> comp = parser.find_component(arco.from(), arco.to());
                double atteso = (comp.type == 'R') ? comp.value : 0.0;
                if (fabs(mats.R(r, r) - atteso) > 1e-9)
                {
                    cerr << "  R(" << r << "," << r << ") = " << mats.R(r, r)
                         << ", atteso " << atteso << endl;
                    return EXIT_FAILURE;
                }
            }

            for (int i = 0; i < mats.R.rows(); ++i)
                for (int j = 0; j < mats.R.cols(); ++j)
                    if (i != j && fabs(mats.R(i, j)) > 1e-9)
                    {
                        cerr << "  R non diagonale: R(" << i << "," << j << ") = "
                             << mats.R(i, j) << endl;
                        return EXIT_FAILURE;
                    }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Ogni elemento di B vale -1, 0 o +1 ed e' diverso da 0 se e solo se
    // il ramo e' resistivo ed e' attivo nella maglia corrispondente.
    int test_B_is_signed_incidence()
    {
        try
        {
            input<int> parser = parse_circuit("test_matrices_A.txt", CIRCUIT_A);
            Graph<int> graph = parser.get_graph();
            list<Cycles<int>> base = mesh_base(graph);
            CircuitMatrices mats = build_matrices(graph, base, parser);

            int col = 0;
            for (const auto &ciclo : base)
            {
                for (const auto &arco : graph.all_edges())
                {
                    unsigned int r = graph.edge_number(arco);
                    double b = mats.B(r, col);

                    if (b != -1.0 && b != 0.0 && b != 1.0)
                    {
                        cerr << "  B(" << r << "," << col << ") = " << b
                             << " non in {-1,0,1}" << endl;
                        return EXIT_FAILURE;
                    }

                    Component<int> comp = parser.find_component(arco.from(), arco.to());
                    bool nonzero_atteso = (comp.type == 'R') && ciclo.is_active(arco);
                    if (nonzero_atteso && b == 0.0)
                    {
                        cerr << "  B(" << r << "," << col << ") atteso != 0" << endl;
                        return EXIT_FAILURE;
                    }
                    if (!nonzero_atteso && b != 0.0)
                    {
                        cerr << "  B(" << r << "," << col << ") atteso 0, ottenuto "
                             << b << endl;
                        return EXIT_FAILURE;
                    }
                }
                col++;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Senza generatori il vettore v deve essere nullo.
    int test_v_is_zero_without_generators()
    {
        try
        {
            input<int> parser = parse_circuit("test_matrices_B.txt", CIRCUIT_B);
            Graph<int> graph = parser.get_graph();
            list<Cycles<int>> base = mesh_base(graph);
            CircuitMatrices mats = build_matrices(graph, base, parser);

            for (int j = 0; j < mats.v.size(); ++j)
                if (fabs(mats.v(j)) > 1e-9)
                {
                    cerr << "  v(" << j << ") = " << mats.v(j) << ", atteso 0" << endl;
                    return EXIT_FAILURE;
                }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Su un circuito noto la soluzione del sistema A I = v deve riprodurre le
    // correnti e le tensioni attese sui rami resistivi.
    int test_solution_on_known_circuit()
    {
        try
        {
            input<int> parser = parse_circuit("test_matrices_A.txt", CIRCUIT_A);
            Graph<int> graph = parser.get_graph();
            list<Cycles<int>> base = mesh_base(graph);
            CircuitMatrices mats = build_matrices(graph, base, parser);

            Eigen::MatrixXd A = mats.B.transpose() * mats.R * mats.B;
            Eigen::LLT<Eigen::MatrixXd> chol(A);
            if (chol.info() != Eigen::Success)
            {
                cerr << "  A non e' definita positiva" << endl;
                return EXIT_FAILURE;
            }
            Eigen::VectorXd I_maglie = chol.solve(mats.v);
            Eigen::VectorXd I_rami = mats.B * I_maglie;
            Eigen::VectorXd V_rami = mats.R * I_rami;

            unsigned int r_R1 = graph.edge_number(Edge<int>(2, 3)); // 2 ohm
            unsigned int r_R2 = graph.edge_number(Edge<int>(1, 3)); // 3 ohm

            // corrente di maglia |I| = 10 / (2+3) = 2 A su entrambi i rami in serie
            if (fabs(fabs(I_rami(r_R1)) - 2.0) > 1e-6 ||
                fabs(fabs(I_rami(r_R2)) - 2.0) > 1e-6)
            {
                cerr << "  correnti attese 2 A: I(R1)=" << I_rami(r_R1)
                     << " I(R2)=" << I_rami(r_R2) << endl;
                return EXIT_FAILURE;
            }
            // V = R * I  ->  R1: 2*2 = 4 V,  R2: 3*2 = 6 V
            if (fabs(fabs(V_rami(r_R1)) - 4.0) > 1e-6 ||
                fabs(fabs(V_rami(r_R2)) - 6.0) > 1e-6)
            {
                cerr << "  tensioni attese 4 V e 6 V: V(R1)=" << V_rami(r_R1)
                     << " V(R2)=" << V_rami(r_R2) << endl;
                return EXIT_FAILURE;
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }
};

int main()
{
    MatricesTest t;

    if (t.test_matrix_dimensions() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_matrix_dimensions" << endl;
    if (t.test_R_is_diagonal_resistances() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_R_is_diagonal_resistances" << endl;
    if (t.test_B_is_signed_incidence() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_B_is_signed_incidence" << endl;
    if (t.test_v_is_zero_without_generators() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_v_is_zero_without_generators" << endl;
    if (t.test_solution_on_known_circuit() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_solution_on_known_circuit" << endl;

    return EXIT_SUCCESS;
}
