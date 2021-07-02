
@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@1 = private unnamed_addr constant [32 x i8] c"Hello World For Fibol Language!\00", align 1
@2 = private unnamed_addr constant [18 x i8] c"magic number is: \00", align 1
@3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

define void @main() {
start:
  %calltmp = call i64 @PrintOutLn(i64 ptrtoint ([1 x i8]* @0 to i64))
  %calltmp1 = call i64 @PrintOutLn(i64 ptrtoint ([32 x i8]* @1 to i64))
  %calltmp2 = call i64 @PrintOutLn(i64 ptrtoint ([18 x i8]* @2 to i64))
  %calltmp3 = call i64 @fibonacci(i64 100)
  %calltmp4 = call i64 @PrintOutI(i64 %calltmp3)
  %calltmp5 = call i64 @PrintOutLn(i64 ptrtoint ([1 x i8]* @3 to i64))
  ret void
}

declare i64 @New(i64)

declare i64 @Delete(i64)

declare i64 @PrintOutLn(i64)

declare i64 @PrintOut(i64)

declare i64 @PrintOutC(i64)

declare i64 @PrintOutI(i64)

declare i64 @Fibol()

define i64 @fibonacci(i64 %n) {
entry:
  %n1 = alloca i64, align 8
  store i64 %n, i64* %n1, align 4
  %greater = icmp ugt i64 %n, 1
  br i1 %greater, label %then, label %else

then:                                             ; preds = %entry
  %subtmp = sub i64 %n, 1
  %calltmp = call i64 @fibonacci(i64 %subtmp)
  %addtmp = add i64 %n, %calltmp
  ret i64 %addtmp

else:                                             ; preds = %entry
  ret i64 1

ifcont:                                           ; No predecessors!
  ret i64 0
}
