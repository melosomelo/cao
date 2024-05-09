# Computer Architecture and Organization (cao)

This repository is a collection of [SystemC](https://systemc.org/) implementations and explanations
of hardware components that I wrote while taking a course in Computer Organization/Architecture
at [UFRN](https://en.wikipedia.org/wiki/Federal_University_of_Rio_Grande_do_Norte).

To run the project, compile it with `make` and then run with `./proc`. The program accepts two optional command
line arguments: `--data` and `--instruction`. These are files that should hold, respectively, the contents
from the data memory and the instruction memory. You can use the ones in the `examples` folder. The command
`./proc --data examples/beq/datamem.dat --instruction examples/beq/instmem.hex` would start the simulation
by loading the memories with the contents of the `examples/beq` folder.
