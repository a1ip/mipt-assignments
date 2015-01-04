using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace arg_task {
  class Program {
    static void Main(string[] args) {
      if (args.Length == 0) {
        Console.WriteLine("Execute this program with -h argument to get help.");
        return;
      }
      switch (args[0]) {
        case "-h":
          Console.WriteLine("The cake is a lie.");
          break;
        case "-d":
          if (args.Length == 1) {
            Console.WriteLine("There should be at least one numerical argument after -d.");
            return;
          }
          int number;
          if (!Int32.TryParse(args[1], out number)) {
            Console.WriteLine("There should be at least one numerical argument after -d.");
            return;
          }
          Console.WriteLine(number);
          for (int i = 2; i < args.Length; i++) {
            Console.WriteLine(args[i]);
          }
          break;
      }
      return;
    }
  }
}
