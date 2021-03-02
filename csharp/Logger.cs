using System;

namespace HashCode {
    public static class Logger {
        private static bool write = false;
        
        public static void Init(bool writeToConsole) {
            write = writeToConsole;
        }

        public static void Divider() {
            if (!write) return;
            Console.WriteLine();
        }

        public static void Log(string format, params object[] args) {
            if (!write) return;
            Console.WriteLine(format, args);
        }
    }
}