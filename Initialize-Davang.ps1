# davang esp idf 

$DAVANG_ESP="$HOME\esp-davang"
$DAVANG_COMPONENT_DIRS="$DAVANG_ESP\components"

$env:DAVANG_ESP="$DAVANG_ESP"
$env:DAVANG_COMPONENT_DIRS="$DAVANG_COMPONENT_DIRS"

powershell.exe -ExecutionPolicy Bypass -NoExit -File "$HOME\Espressif\Initialize-Idf.ps1" -IdfId esp-idf-7c1874b8bb4030737690c6e171497564