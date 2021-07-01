
define void @main() {
start:
  %calltmp = call i64 @HelloWorld(i64 1, i64 2)
  ret void
}

define i64 @HelloWorld(i64 %a, i64 %b) {
entry:
  %c = alloca i64, align 8
  %b2 = alloca i64, align 8
  %a1 = alloca i64, align 8
  store i64 %a, i64* %a1, align 4
  store i64 %b, i64* %b2, align 4
  %a3 = load i64, i64* %a1, align 4
  %b4 = load i64, i64* %b2, align 4
  %greater = icmp ugt i64 %a3, %b4
  br i1 %greater, label %then, label %else13

then:                                             ; preds = %entry
  %a5 = load i64, i64* %a1, align 4
  %b6 = load i64, i64* %b2, align 4
  %greater7 = icmp ugt i64 %a5, %b6
  br i1 %greater7, label %then8, label %else

then8:                                            ; preds = %then
  %a9 = load i64, i64* %a1, align 4
  %b10 = load i64, i64* %b2, align 4
  %addtmp = add i64 %a9, %b10
  store i64 %addtmp, i64* %c, align 4
  %c11 = load i64, i64* %c, align 4
  store i64 %c11, i64* %a1, align 4
  %a12 = load i64, i64* %a1, align 4
  ret i64 %a12
  br label %ifcont

else:                                             ; preds = %then
  ret i64 10
  br label %ifcont

ifcont:                                           ; preds = %else, %then8
  br label %ifcont14

else13:                                           ; preds = %entry
  br label %ifcont14

ifcont14:                                         ; preds = %else13, %ifcont
  ret i64 0
}

define i64 @ByeWorld(i64 %a, i64 %b) {
entry:
  %b2 = alloca i64, align 8
  %a1 = alloca i64, align 8
  store i64 %a, i64* %a1, align 4
  store i64 %b, i64* %b2, align 4
  %a3 = load i64, i64* %a1, align 4
  %b4 = load i64, i64* %b2, align 4
  %multmp = mul i64 %a3, %b4
  ret i64 %multmp
  ret i64 0
}
