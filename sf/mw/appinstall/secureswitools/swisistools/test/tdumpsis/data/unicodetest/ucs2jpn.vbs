Dim fso
Dim File

Set fso = CreateObject("Scripting.FileSystemObject")
Set File = fso.CreateTextFile("\epoc32\winscw\c\tswi\ﾅｿﾄ.txt", true)
File.WriteLine("Sample File.")
File.Close()

Set WshShell = CreateObject("WScript.Shell")
Set oExec = WshShell.Exec("makesis ..\ucs2jpn.pkg ucs2jpn.sis")
Do While oExec.Status = 0 
	WScript.Sleep 100 
Loop 

Set onExec = WshShell.Exec("dumpsis -x ucs2jpn.sis")

Do While onExec.Status = 0 
	WScript.Sleep 100 
Loop 

if (fso.FolderExists("ucs2jpn") And fso.FileExists("ucs2jpn\ucs2jpn.pkg") And fso.FileExists("ucs2jpn\file0")) then
	Set File = fso.CreateTextFile("\epoc32\winscw\c\tswi\tdumpsis\passed.txt", true)
     	File.WriteLine ("Passed")
     	File.Close()
End If
