Dim WshShell, oExec
Dim fso_result, fso_pkg 

Set fso = CreateObject("Scripting.FileSystemObject")
'Cleanup routine for all files created

if (fso.FileExists("ucs2jpn.sis")) then
	fso.DeleteFile "ucs2jpn.sis ", true
End If

if (fso.FileExists("\epoc32\winscw\c\tswi\tdumpsis\ﾅｿﾄ.txt")) then
	fso.DeleteFile "\epoc32\winscw\c\tswi\tdumpsis\ﾅｿﾄ.txt", true
End If

if (fso.FileExists("\epoc32\winscw\c\tswi\tdumpsis\passed.txt")) then
	fso.DeleteFile "\epoc32\winscw\c\tswi\tdumpsis\passed.txt", true
End If


if (fso.FolderExists("\epoc32\winscw\c\tswi\tdumpsis\ucs2jpn")) then
	fso.DeleteFolder "\epoc32\winscw\c\tswi\tdumpsis\ucs2jpn", true
End If

