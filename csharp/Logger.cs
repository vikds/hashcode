using System;

namespace HashCode {
    public static class Logger {
        private static bool _write = false;
        
        public static void Init(bool writeToConsole) {
            _write = writeToConsole;
        }

        public static void Divider() {
            if (!_write) return;
            Console.WriteLine();
        }

        public static void Debug(string format, params object[] args) {
            if (!_write) return;
            Console.WriteLine(format, args);
        }
        
        public static void Info(string format, params object[] args) {
            Console.WriteLine(format, args);
        }
    }
}