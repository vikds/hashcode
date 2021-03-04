# Google HashCode just for fun

## Online Qualifications 2021

We've taken part in solving Online Qualiffications of Hash Code 2021 problem `"Traffic signaling"` for the first time. Our result was `8,446,233` points, while the best score was `10,586,135`. We would like to improve our result, and even arrange a small competition between ourselves. That's all just for fun.

## To run C++ code

```bash
$ cd cpp
$ mkdir build
$ cd build
$ cmake ..
...
-- Build files have been written to: ~/hashcode/cpp/build
$ make
...
[100%] Built target hashcode
$ ./hashcode
Usage: ./hashcode [-io] [-i input][-o output]
...
```

## C++ results

    1. A – An example: 2,000 points
    2. B – By the ocean: 4,566,658 points
    3. C – Checkmate: 1,298,533 points
    4. D – Daily commute: 1,567,509 points
    5. E – Etoile: 705,093 points
    6. F – Forever jammed: 1,342,639 points

    Total: 9,482,432 points
    
## Building C# code
[.NET 5.0](https://dotnet.microsoft.com/download/dotnet/5.0) is required to build C# code located in `./csharp` folder. It's a completely separated version of the solution and not related in any way to a C++ code.

To build on Linux
```shell
$ cd csharp
$ dotnet publish ./HashCode.csproj \
   --output ./bin \
   --configuration Release \
   --runtime linux-x64 \
   -p:GenerateFullPaths=true \
   -p:PublishReadyToRun=true \
   -p:PublishSingleFile=true \
   --self-contained false 
```

Usage:
```shell
$ ./bin/HashCode -i ../input/a.txt -o ./output
```

Required parameters: 
- `-i|--input <input file>` - loads model from the specified file
- `-o|--output <output folder>` - puts the result here

Optional parameter:
- `-v|--verbose` - prints detailed simulation steps 


To simply compile and run C# code without publishing:
```shell
$ cd csharp
$ dotnet run -i input.txt -o ./output
```

## C# results

|   | Name | Score |
| - | - | - |
| 1 | A – An example | 2 000 |
| 2 | B – By the ocean | 4 559  991 |
| 3 | C – Checkmate | TBP |
| 4 | D – Daily commute | TBP |
| 5 | E – Etoile | TBP |
| 6 | F – Forever jammed | TBP |

Total: TBP
