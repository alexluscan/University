using System;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace pdp_lab4;

internal static class Program
{
    private static readonly DownloadRequest[] Targets =
    {
        new("example.com", 80, "/", "Example.com root"),
        new("example.org", 80, "/", "Example.org root"),
        new("example.net", 80, "/", "Example.net root"),
    };

    public static async Task Main(string[] args)
    {
        PrintMenu();
        var choice = Console.ReadLine();

        if (choice == "1")
        {
            Console.WriteLine("=== Callback solution ===");
            var handles = DirectCallbacks.StartDownloads(Targets);
            WaitHandle.WaitAll(handles);
            foreach (var handle in handles)
            {
                handle.Dispose();
            }
        }
        else if (choice == "3")
        {
            Console.WriteLine("=== Async/await solution ===");
            await AsyncAwaitMechansism.RunAsync(Targets);
        }
        else
        {
            Console.WriteLine("Invalid choice.");
            return;
        }

        Console.WriteLine("Downloads completed.");
    }

    private static void PrintMenu()
    {
        Console.WriteLine("1. Callback solution");
        Console.WriteLine("3. Async/Await solution");
        Console.Write("Select an option: ");
    }
}

