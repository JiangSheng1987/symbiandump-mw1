Dim WshShell, oExec
Dim fso 

' Create a normal sis file

Set fso = CreateObject("Scripting.FileSystemObject")
Set File = fso.CreateTextFile("ゎわこんァア龐龑.pkg", true)
File.WriteLine (";Languages")
File.WriteLine ("&JA")
File.WriteLine ("")
File.WriteLine (";Header")
File.WriteLine ("#{""names large sample UTF8""}, (0xA2000222), 1, 2, 3, TYPE=SA")
File.WriteLine ("")
File.WriteLine ("%{""Vendor""}")
File.WriteLine (":""Vendor""")
File.WriteLine ("")
File.WriteLine (";Mixed UTF8 characters")
File.WriteLine ("""\epoc32\winscw\c\tswi\utf8.txt""-""!:\utf8.txt""")
File.Close()

Set WshShell = CreateObject("WScript.Shell")
Set oExec = WshShell.Exec("makesis ゎわこんァア龐龑.pkg")
Do While oExec.Status = 0 
	WScript.Sleep 100 
Loop 

' Create a sis file embedded with the previous one created

Set fso = CreateObject("Scripting.FileSystemObject")
Set File = fso.CreateTextFile("丁丂七丄丟両丣乕乖乗乘乕乖乗乘乕乖乗乘乣乨乩龝龞龠丹主丼世丗丘.pkg", true, true)
File.WriteLine (";Languages")
File.WriteLine ("&JA")
File.WriteLine ("")
File.WriteLine (";Header")
File.WriteLine ("#{""names large sample UTF8""}, (0xA2000222), 1, 2, 3, TYPE=SA")
File.WriteLine ("")
File.WriteLine ("%{""Vendor""}")
File.WriteLine (":""Vendor""")
File.WriteLine ("")
File.WriteLine (";Mixed UTF8 characters")
File.WriteLine ("""\epoc32\winscw\c\tswi\utf8.txt""-""!:\utf8.txt""")
File.WriteLine ("@""ゎわこんァア龐龑.sis"",(0xA2000222)")
File.Close()

Set WshShell = CreateObject("WScript.Shell")
Set oExec = WshShell.Exec("makesis 丁丂七丄丟両丣乕乖乗乘乕乖乗乘乕乖乗乘乣乨乩龝龞龠丹主丼世丗丘.pkg")
Do While oExec.Status = 0 
	WScript.Sleep 100 
Loop 

Set onExec = WshShell.Exec("dumpsis -x 丁丂七丄丟両丣乕乖乗乘乕乖乗乘乕乖乗乘乣乨乩龝龞龠丹主丼世丗丘.sis")

Do While onExec.Status = 0 
	WScript.Sleep 100 
Loop 

if (fso.FolderExists("丁丂七丄丟両丣乕乖乗乘乕乖乗乘乕乖乗乘乣乨乩龝龞龠丹主丼世丗丘") And fso.FileExists("丁丂七丄丟両丣乕乖乗乘乕乖乗乘乕乖乗乘乣乨乩龝龞龠丹主丼世丗丘.pkg") And fso.FileExists("丁丂七丄丟両丣乕乖乗乘乕乖乗乘乕乖乗乘乣乨乩龝龞龠丹主丼世丗丘\file0")) then
	Set File = fso.CreateTextFile("\epoc32\winscw\c\tswi\tdumpsis\passed.txt", true)
     	File.WriteLine ("Passed")
     	File.Close()
End If