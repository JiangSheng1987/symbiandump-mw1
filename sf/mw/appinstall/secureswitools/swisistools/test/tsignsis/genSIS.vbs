Dim fso
Set fso = CreateObject("Scripting.FileSystemObject")
fso.CopyFile "pkg_kanji.SIS", "\epoc32\winscw\c\tswi\tsignsis\pkg_乕乖乗乘_kanji.SIS", true
fso.CopyFile "kanji_key.key", "\epoc32\winscw\c\tswi\tsignsis\丟両丣kanji_key.key", true
fso.CopyFile "kanji_cer.cer", "\epoc32\winscw\c\tswi\tsignsis\丁丂七丄kanji_cer.cer", true


