@rem
@rem Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
@rem All rights reserved.
@rem This component and the accompanying materials are made available
@rem under the terms of the License "Eclipse Public License v1.0"
@rem which accompanies this distribution, and is available
@rem at the URL "http://www.eclipse.org/legal/epl-v10.html".
@rem
@rem Initial Contributors:
@rem Nokia Corporation - initial contribution.
@rem
@rem Contributors:
@rem
@rem Description:
@rem

:: This batch file creates sis file/files
@ECHO OFF
REM testcaseM001a
if not exist .\cdrive mkdir .\cdrive > NUL
if not exist .\romdrive mkdir .\romdrive > NUL
IF NOT EXIST .\romdrive\system\install\ mkdir .\romdrive\system\install\ > NUL
IF NOT EXIST .\cdrive\sys\install\scr\ mkdir .\cdrive\sys\install\scr\ > NUL
call makesis \epoc32\winscw\c\tswi\tinterpretsisinteg\testdatam001\interpretsism001.pkg \epoc32\winscw\c\tswi\tinterpretsisinteg\sisfiles\interpretsism001.sis > NUL
call /epoc32/winscw/c/tswi/tinterpretsisinteg/interpretsism001a_db.bat > NUL
:END