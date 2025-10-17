# LLVM InstCombine : Combine Redundant Instructions
Projekat iz kursa Konstrukcija Kompilatora na Matematičkom fakultetu 2024/25

InstCombine je pass u LLVM kompajlerskoj infrastrukturi koji vrši optimizaciju kombinovanja instrukcija. Cilj mu je da transformiše složene instrukcije u jednostavnije ili efikasnije oblike. Ovo pomaže u smanjenju ukupnog vremena izvršavanja i poboljšava performanse generisanog koda pojednostavljivanjem međureprezentacije (IR) koju koristi LLVM.

### Značaj
InstCombine pass se obično koristi u srednjem delu LLVM optimizacijskog procesa, nakon što su drugi pass-ovi transformisali program u oblik koji može imati koristi od optimizacija na nivou instrukcija.

### Ključne karakteristike:
- **Pojednostavljenje izraza**: Kombinuje instrukcije kao što su sabiranje, množenje, pomeranje, itd.
- **Optimizacija obrazaca**: Zamenjuje poznate, skupe operacije sa bržim alternativama.
- **Constant folding**: Evaluira konstantne izraze u vreme kompajliranja.

---

### Instrukcije za pokretanje `InstCombineNew`:

   ```bash
   clang -S -emit-llvm test.c -o test.ll
   opt -load LLVMInstCombineNew.so -enable-new-pm=0 -instcombine-new test.ll -S -o tmp.ll
