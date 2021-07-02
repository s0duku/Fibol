; ModuleID = 'test.cc'
source_filename = "test.cc"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline optnone uwtable mustprogress
define dso_local i32 @_Z5hellov() #0 {
  %1 = alloca i64, align 8
  %2 = alloca i32*, align 8
  %3 = alloca i32*, align 8
  %4 = alloca i32 (i32, i32)*, align 8
  %5 = load i64, i64* %1, align 8
  %6 = inttoptr i64 %5 to i32 (i32, i32)*
  %7 = call i32 %6(i32 3, i32 4)
  %8 = load i32 (i32, i32)*, i32 (i32, i32)** %4, align 8
  %9 = call i32 %8(i32 1, i32 2)
  ret i32 %9
}

attributes #0 = { noinline optnone uwtable mustprogress "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 12.0.1-++20210508012946+a1a197b54ec6-1~exp1~20210507233651.85"}
