module Parallel

open System
open System.Threading
open System.Threading.Tasks
open MergeSort


let rec parallelDepth xs depth maxDepth: int list =
    if depth < maxDepth
    then 
        let mid = (List.length xs) / 2 
        let (xs', xs'') = List.splitAt mid xs
        let xs1 = Task.Run(fun () -> parallelDepth xs' (depth+1) maxDepth)
        // let xs2 = parallelDepth xs'' (depth+1) maxDepth
        let xs2 = Task.Run(fun () -> parallelDepth xs'' (depth+1) maxDepth)
        merge xs1.Result xs2.Result
    else 
        mergeSort xs

let parallelSort xs nrThreads =
    let maxThreadDepth = Math.Log(nrThreads |> float, 2.) |> int
    parallelDepth xs 0 maxThreadDepth