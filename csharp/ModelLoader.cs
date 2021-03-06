using System;
using System.IO;
using System.Linq;

namespace HashCode {
    static class ModelLoader {
        public static Model ImportModel(string fileName) {
            using var sr = new StreamReader(fileName);
            int[] n = sr
                .ReadLine()
                .Split(' ')
                .Select(int.Parse)
                .ToArray();

            var model = new Model(
                duration: n[0],
                intersectionsNum: n[1],
                streetsNum: n[2],
                carsNum: n[3],
                bonus: n[4]
            );

            for (var i = 0; i < model.Streets.Length; i++) {
                string[] l = sr
                    .ReadLine()
                    .Split(' ');

                model.Streets[i] = new Street() {
                    StartsAt = model.Intersections[int.Parse(l[0])],
                    EndsAt = model.Intersections[int.Parse(l[1])],
                    Name = l[2],
                    Length = int.Parse(l[3]),
                };
                model.StreetMap[model.Streets[i].Name] = model.Streets[i];
                model.Streets[i].StartsAt.Outcoming.Add(model.Streets[i]);
                model.Streets[i].EndsAt.Incoming.Add(model.Streets[i]);
            }

            for (var i = 0; i < model.Cars.Length; i++) {
                string[] l = sr
                    .ReadLine()
                    .Split(' ');
                model.Cars[i] = new Car() {
                    Id = i,
                    Route = l
                        .Skip(1)
                        .Select(s => model.StreetMap[s])
                        .ToArray(),
                };
            }
            return model;
        }

        public static void ExportModel(Model model, string inputFileName, string outputFolderName) {
            var inputFileNameExtension = Path.GetExtension(inputFileName);
            var inputFileNameWithoutExtension = Path.GetFileNameWithoutExtension(inputFileName);

            var outputFileName = Path.Combine(
                outputFolderName,
                $"{inputFileNameWithoutExtension}_{DateTime.Now:yyyy_MM_dd_HH_mm}_Score_{model.Score}"
            );
            outputFileName = Path.ChangeExtension(outputFileName, inputFileNameExtension);

            if (!Directory.Exists(outputFolderName)) {
                Directory.CreateDirectory(outputFolderName);
            }

            using var sw = new StreamWriter(outputFileName);
            sw.Write(model.ExportSchedule());
            sw.Flush();
            sw.Close();
        }
    }
}
