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

        public static void Debug(string value) {
            if (!_write) return;
            Console.WriteLine(value);
        }
        public static void Debug(string format, params object[] args) {
            if (!_write) return;
            Console.WriteLine(format, args);
        }

        public static void Debug(string format, int arg0, string arg1, int arg2) {
            if (!_write) return;
            Console.WriteLine(format, arg0.ToString(), arg1, arg2.ToString());
        }

        public static void Debug(string format, string arg0, TrafficLight.Colors arg1, int arg2) {
            if (!_write) return;
            Console.WriteLine(format, arg0, arg1.ToString(), arg2.ToString());
        }

        public static void Debug(string format, int arg0, int arg1, int arg2, int arg3, int arg4) {
            if (!_write) return;
            Console.WriteLine(format, arg0.ToString(), arg1.ToString(), arg2.ToString(), arg3.ToString(), arg4.ToString());
        }

        public static void Debug(string format, int arg) {
            if (!_write) return;
            Console.WriteLine(format, arg.ToString());
        }

        public static void Info(string format, params object[] args) {
            Console.WriteLine(format, args);
        }
    }
}
