perl ../doxygraph/doxygraph/doxygraph xml/index.xml graph.dot
dot -Tpdf graph.dot -o graph.pdf
