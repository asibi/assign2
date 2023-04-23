module Main
open Parallel

let genRandomNumbers count =
    let rnd = System.Random()
    List.init count (fun _ -> rnd.Next ())

let isSorted l = l |> Seq.pairwise |> Seq.forall (fun (a, b) -> a <= b)

let measure f xs nrThreads =
    let stopWatch = System.Diagnostics.Stopwatch.StartNew()
    let xs' = f xs nrThreads
    stopWatch.Stop()
    printfn "%A" (isSorted xs')
    stopWatch.Elapsed.TotalMilliseconds

[<EntryPoint>]
let main args =
    // printfn "Arguments passed to function : %A" args

    let nrThreads = args.[0] |> int
    let xs = genRandomNumbers 10000000

    let time = measure parallelSort xs nrThreads
    printfn "%.2fms" time
    0 // Return 0. This indicates success.