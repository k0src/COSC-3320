param([string]$file)
$exe = [System.IO.Path]::ChangeExtension($file, ".exe")
g++ -o $exe $file