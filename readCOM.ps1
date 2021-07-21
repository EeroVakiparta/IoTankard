$port= new-Object System.IO.Ports.SerialPort "COM8", 9600, None, 8, one
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
    }
}