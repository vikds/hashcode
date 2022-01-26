using System;
using System.Linq;

namespace HashCode {
    public class Parameters {
        public Parameters(string[] args) {
            var argsList = args.ToList();
            if (argsList.Any(a => a == "-v" || a == "--v" || a == "-verbose" || a == "--verbose")) {
                Verbose = true;
            }
            var p = argsList.FindIndex(a => a == "-p" || a == "--p" || a == "-parallel" | a == "--parallel");
            if (p > 0) {
                Parallel = true;
                ParallelSimulations = -1;
                if (p < argsList.Count - 1) {
                    if (int.TryParse(argsList[p + 1], out int parallel)) {
                        ParallelSimulations = parallel;
                    } else {
                        throw new Exception("Cannot read max parallel simulations");
                    }
                }
            }
            var i = argsList.FindIndex(a => a == "-i" || a == "--i" || a == "-input" | a == "--input");
            if (i < 0) {
                throw new Exception("No input file specified");
            }
            if (i == argsList.Count - 1) {
                throw new Exception("No input file specified");
            }
            InputFile = argsList[i + 1];

            var o = argsList.FindIndex(a => a == "-o" || a == "--o" || a == "-output" | a == "--output");
            if (o < 0) {
                throw new Exception("No output folder specified");
            }
            if (o == argsList.Count - 1) {
                throw new Exception("No output folder specified");
            }
            OutputFolder = argsList[o + 1];
        }

        public bool Verbose { get; private set; }
        public string InputFile { get; private set; }
        public string OutputFolder { get; private set; }
        public bool Parallel { get; private set; }
        public int ParallelSimulations { get; private set; }
    }
}
