using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class IDTFactory : KernelTableFactory
    {
        public IDTFactory()
        {
            CTLCode = CTLCodeUtility.GetCTLCode(DeviceType.FILE_DEVICE_UNKNOWN, 0x800, MethodType.METHOD_BUFFERED, AccessType.FILE_ANY_ACCESS);
        }

        public KernelTable CreateIDT(DeviceDataTransmitter deviceDataTransmitter, uint processorNumber)
        {
            byte[] data = deviceDataTransmitter.ExecDeviceIoControl(CTLCode, BitConverter.GetBytes(processorNumber));
            return IDT.Parse(processorNumber, data, 0);
        }
    }
}
