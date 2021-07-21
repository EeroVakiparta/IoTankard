[System.IO.Ports.SerialPort]::GetPortNames()
$portName = Read-Host -Prompt 'Pick port from list:'

$port= new-Object System.IO.Ports.SerialPort $portName, 9600, None, 8, one
$port.Open()

Write-Output $port

for(;;)
{
    if ($port.IsOpen)
    {
        $data = $port.ReadLine()
        Add-Type -AssemblyName System.Windows.Forms
        [System.Windows.Forms.SendKeys]::SendWait($data);
        Write-Output $data
        if ($data -eq "quit"){
            break;
        }
    }
}