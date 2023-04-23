import util.Random
import scala.annotation.tailrec
import scala.concurrent.Future
import scala.concurrent.ExecutionContext.Implicits.global


def genList(count: Int): List[Int] =
    Seq.fill(count)(Random.nextInt).toList

def sorted(l: List[Int]): Boolean = l == l.sorted

@tailrec
def merge(xs: List[Int], ys: List[Int], acc: List[Int]): List[Int] = 
    (xs, ys) match 
        case (xs, Nil) => acc ++ xs
        case (Nil, ys) => acc ++ ys
        case (x::xss, y::yss) => 
            if (x < y) merge(xss, ys, acc :+ x)
            else merge(xs, yss, acc :+ y)

def mergesort(xs: List[Int]): List[Int] = 
    xs match // todo should i not use size
        case Nil => xs
        case x::Nil => xs
        case _ => 
            val mid = xs.size / 2
            val (fst, snd) = xs.splitAt(mid)
            merge(mergesort(fst), mergesort(snd), Nil)

// def mergeFutureLists[X]( fst: Future[List[Int]], snd: Future[List[Int]] ) = {
//   for{
//     fstRes <- fst
//     sndRes <- snd
//   } yield (fstRes ::: fstRes)
// }
// def mergeFutureLists2[X]( fl1: Future[List[X]], fl2: Future[List[X]]) = {
//   Future.reduce(List(fl1, fl2))(_ ++ _)
// }

// def parallel(xs: List[Int], depth: Int): List[Int] =
//     if (depth < 1)
//         val mid = xs.size / 2
//         val (fst, snd) = xs.splitAt(mid)
//         mergeFutureLists(Future {parallel(fst, depth+1)}, Future {parallel(snd, depth+1)})
//             .isCompleted
//     else
//         mergesort(xs)

@main def run(): Unit =
    val xs = genList(10000)
    val test = mergesort(xs)
    println("Done")
    println(sorted(test))
