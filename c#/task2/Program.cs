using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using NDesk.Options;

using ICSharpCode.SharpZipLib.Core;
using ICSharpCode.SharpZipLib.Zip;
using ICSharpCode.SharpZipLib.GZip;

namespace archive {
  class Program {
    static void Main(string[] args) {
      bool show_help = false;
      bool compress = false;
      string method = "GZIP";
      List<string> files = new List<string>();
      string archive = "";

      var options = new OptionSet() {
        { "m|method=", "compression format. supported formats: ZIP, BZIP, GZIP.",
          v => method = v },
        { "i|input=", "the file to (de)compress.",
          v => { files.Add(v); compress = true; } },
        { "h|help", "show this message and exit.",
          v => show_help = true }
      };

      List<string> extra;
      try {
        extra = options.Parse(args);
      } catch (OptionException e) {
        Console.WriteLine("Exception: " + e.Message);
        PrintHelp(options);
        return;
      }

      if (show_help) {
        PrintHelp(options);
        return;
      }

      if (extra.Count() != 1) {
        Console.WriteLine("Error: you should specify archive name.");
        PrintHelp(options);
        return;
      } else {
        archive = extra[0];
      }

      if (method != "ZIP") {
        Console.WriteLine("This method is not supported yet! Try ZIP.");
        return;
      }

      try {
        if (compress) {
          CompressZip(archive, files);
        } else {
          DecompressZip(archive);
        }
      } catch (Exception e) {
        Console.WriteLine(e.Message);
      }
      return;
    }

    static void PrintHelp(OptionSet options) {
      Console.WriteLine("Usage: archive [OPTIONS]+ archive");
      Console.WriteLine("(De)compresses files using a specified compression format.");
      Console.WriteLine();
      Console.WriteLine("Options:");
      options.WriteOptionDescriptions(Console.Out);
    }

    static void CompressZip(string archive, List<string> files) {
      FileStream output_stream = File.Create(archive);
      ZipOutputStream zip_stream = new ZipOutputStream(output_stream);

      foreach (string file in files) {
        FileInfo file_info = new FileInfo(file);
        string entry_name = ZipEntry.CleanName(file);
        ZipEntry zip_entry = new ZipEntry(entry_name);
        zip_entry.DateTime = file_info.LastWriteTime;
        zip_entry.Size = file_info.Length;
        zip_stream.PutNextEntry(zip_entry);

        byte[] buffer = new byte[4096];
        using (FileStream input_stream = File.OpenRead(file)) {
          StreamUtils.Copy(input_stream, zip_stream, buffer);
        }
        zip_stream.CloseEntry();
      }

      zip_stream.IsStreamOwner = true;
      zip_stream.Close();
    }

    static void DecompressZip(string archive) {
      FileStream input_stream = File.OpenRead(archive);
      ZipFile zip_file = new ZipFile(input_stream);

      foreach (ZipEntry zip_entry in zip_file) {
        if (!zip_entry.IsFile) {
          continue;
        }

        Stream zip_stream = zip_file.GetInputStream(zip_entry);
        String entry_name = zip_entry.Name;

        byte[] buffer = new byte[4096];
        using (FileStream output_stream = File.Create(entry_name)) {
          StreamUtils.Copy(zip_stream, output_stream, buffer);
        }
      }
      
      zip_file.IsStreamOwner = true;
      zip_file.Close();
    }
  }
}
