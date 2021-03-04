using System;
using System.Linq;

namespace HashCode {
    public class Parameters {
        public Parameters(string[] args) {
            var argsList = args.ToList();
            if (argsList.Any(a => a == "-v" || a == "--v" || a == "-verbose" || a == "--verbose")) {
                Verbose = true;
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
    }
}
