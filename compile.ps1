if (-not (Test-Path -Path .\bin -PathType Container)) {
    mkdir .\bin
}

$CFILES = Get-ChildItem -Path *.c | Select-Object -ExpandProperty FullName
gcc $CFILES -o .\bin\interpreter.exe