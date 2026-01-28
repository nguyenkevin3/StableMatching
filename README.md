Project: Stable Matching Algorithm and Verifier

Team:
  1) Kevin Nguyen 93349993
  2) Noelio Perez 77647747

Instructions to compile: **g++ required to build**
  1) Clone the repository
  2) Navigate to project directory and then to src/ directory
  3) Run: g++ -std=c++17 matchingengine.cpp -o matchingengine
  4) Drag and drop input files into the directory (example.in is already provided)
  5) Input files must be of ".in" type
  6) Run: g++ -std=c++17 verifier.cpp -o verifier

Instructions to use matcher:
  1) Run: ./matchingengine [input file] (Ex: ./matchingengine example.in)
  2) An output file will be automatically generated in the same directory

Instructions to use verifier:
  1) Run: ./verifier [output file] [input file] (Ex: ./verifier example.out example.in)
  2) [output file] to use is the one generated from the matchingengine
  3) Validity and stability results will be displayed

Scalability:
![Matching Engine Graph](./graphs/engine.png)
![Verifier Graph](./graphs/verifier.png)
The trend in both graphs is clearly polynomial, approximately close to $O(n^2)$
