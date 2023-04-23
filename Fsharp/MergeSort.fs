module MergeSort

let merge xs ys = 
    let rec mrg xs ys cont = 
        match xs,ys with
        | [],[] -> cont []
        | xs,[] -> cont xs
        | [],ys -> cont ys
        | (x::xs', y::ys')  -> 
            if x < y
            then mrg xs' ys (fun rs -> cont(x::rs))
            else mrg xs ys' (fun rs -> cont(y::rs))
    mrg xs ys (fun xs -> xs)

let mergeSort xs =
    let rec msort xs cont =
        match List.length xs with // todo is it faster without list.length
        | 1 -> cont xs
        | _ -> 
            let mid = (List.length xs) / 2
            let (xs', xs'') = List.splitAt mid xs
            msort xs' (fun ys' -> msort xs'' (fun ys'' -> cont(merge ys' ys'')))
    msort xs (fun xs -> xs)