#include <iostream>
#include <fstream>
#include <set>
#include <cstdlib>
#include <stdexcept>
#include "../edge.h"
#include "../graph.h"
#include "../input.hpp"

using namespace std;

// Test per la classe input (input.hpp): parsing del netlist, validazione
// (niente componenti in parallelo tra due nodi), pulizia del grafo (rami
// aperti / nodi isolati) e recupero dei componenti a partire dai nodi.
class InputTest
{
    // scrive il contenuto su file e ritorna un parser con il file gia' letto
    static input<int> parse(const string &path, const string &content)
    {
        {
            ofstream ofs(path);
            ofs << content;
        }
        input<int> parser;
        if (!parser.parse_file(path))
            throw runtime_error("parse_file ha fallito inaspettatamente");
        return parser;
    }

public:
    int test_parse_file_missing_file_returns_false()
    {
        input<int> parser;
        if (parser.parse_file("file_che_non_esiste.txt"))
            return EXIT_FAILURE;
        return EXIT_SUCCESS;
    }

    int test_parse_file_reads_components()
    {
        try
        {
            input<int> parser = parse("test_input_basic.txt",
                                       "R1 1 1 2\nR2 2 2 3\nR3 3 1 3\n");
            Component<int> c = parser.find_component(1, 2);
            if (c.id != "R1" || c.type != 'R' || c.value != 1.0)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // "ignora i componenti in cortocircuito su se stessi": il nodo 4, citato
    // solo dall'auto-arco, non deve comparire nel grafo risultante.
    int test_parse_file_skips_self_loop()
    {
        try
        {
            input<int> parser = parse("test_input_selfloop.txt",
                                       "R1 1 1 2\nR2 2 2 3\nR3 3 1 3\nX1 5 4 4\n");
            Graph<int> graph = parser.get_graph();
            if (graph.all_nodes().contains(4))
            {
                cerr << "  il nodo 4, introdotto solo dall'auto-arco, non doveva esistere" << endl;
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

    // find_component normalizza la coppia di nodi: l'ordine non conta.
    int test_find_component_is_order_independent()
    {
        try
        {
            input<int> parser = parse("test_input_order.txt", "R1 7 1 2\n");
            Component<int> a = parser.find_component(1, 2);
            Component<int> b = parser.find_component(2, 1);
            if (a.id != b.id || a.value != b.value)
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    int test_find_component_unknown_pair_throws()
    {
        try
        {
            input<int> parser = parse("test_input_unknown.txt", "R1 1 1 2\n");
            parser.find_component(1, 99);
            return EXIT_FAILURE; // doveva lanciare
        }
        catch (const out_of_range &)
        {
            return EXIT_SUCCESS;
        }
    }

    // Triangolo: nessuna coppia di nodi ha due componenti -> configurazione valida.
    int test_check_validity_accepts_circuit_without_parallel()
    {
        try
        {
            input<int> parser = parse("test_input_valid.txt",
                                       "R1 1 1 2\nR2 2 2 3\nR3 3 1 3\n");
            if (!parser.check_validity())
                return EXIT_FAILURE;
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // R1 e R4 sono entrambi tra i nodi 1 e 2: configurazione non consentita.
    int test_check_validity_rejects_parallel_components()
    {
        try
        {
            input<int> parser = parse("test_input_parallel.txt",
                                       "R1 1 1 2\nR2 2 2 3\nR3 3 1 3\nR4 4 1 2\n");
            if (parser.check_validity())
            {
                cerr << "  R1 e R4 sono in parallelo: check_validity doveva fallire" << endl;
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

    // Il ramo pendente verso il nodo 4 (foglia, grado 1) deve essere rimosso;
    // resta solo il triangolo 1-2-3.
    int test_clean_graph_removes_leaf_branch()
    {
        try
        {
            input<int> parser = parse("test_input_leaf.txt",
                                       "R1 1 1 2\nR2 2 2 3\nR3 3 1 3\nR4 4 2 4\n");
            parser.clean_graph();
            Graph<int> graph = parser.get_graph();

            if (graph.all_nodes() != set<int>{1, 2, 3})
            {
                cerr << "  nodi attesi {1,2,3}, ottenuti " << graph.all_nodes() << endl;
                return EXIT_FAILURE;
            }
            try
            {
                parser.find_component(2, 4);
                cerr << "  il componente sul ramo foglia doveva essere rimosso" << endl;
                return EXIT_FAILURE;
            }
            catch (const out_of_range &)
            {
                // atteso
            }
            return EXIT_SUCCESS;
        }
        catch (const exception &e)
        {
            cerr << "  eccezione: " << e.what() << endl;
            return EXIT_FAILURE;
        }
    }

    // Un grafo che e' solo un cammino (nessun ciclo) si riduce al vuoto:
    // ogni ramo ha sempre almeno un estremo di grado 1.
    int test_clean_graph_on_pure_path_results_in_empty_graph()
    {
        try
        {
            input<int> parser = parse("test_input_path.txt", "R1 1 1 2\nR2 2 2 3\n");
            parser.clean_graph();
            Graph<int> graph = parser.get_graph();
            if (!graph.all_nodes().empty() || !graph.all_edges().empty())
            {
                cerr << "  grafo atteso vuoto dopo la pulizia" << endl;
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
    InputTest t;

    if (t.test_parse_file_missing_file_returns_false() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_parse_file_missing_file_returns_false" << endl;
    if (t.test_parse_file_reads_components() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_parse_file_reads_components" << endl;
    if (t.test_parse_file_skips_self_loop() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_parse_file_skips_self_loop" << endl;
    if (t.test_find_component_is_order_independent() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_find_component_is_order_independent" << endl;
    if (t.test_find_component_unknown_pair_throws() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_find_component_unknown_pair_throws" << endl;
    if (t.test_check_validity_accepts_circuit_without_parallel() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_check_validity_accepts_circuit_without_parallel" << endl;
    if (t.test_check_validity_rejects_parallel_components() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_check_validity_rejects_parallel_components" << endl;
    if (t.test_clean_graph_removes_leaf_branch() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_clean_graph_removes_leaf_branch" << endl;
    if (t.test_clean_graph_on_pure_path_results_in_empty_graph() != EXIT_SUCCESS)
        return EXIT_FAILURE;
    cout << "test_clean_graph_on_pure_path_results_in_empty_graph" << endl;

    return EXIT_SUCCESS;
}
