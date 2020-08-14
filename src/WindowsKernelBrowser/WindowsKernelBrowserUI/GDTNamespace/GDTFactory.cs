using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class GDTFactory : KernelTableFactory
    {
        public GDTFactory()
        {
            CTLCode = CTLCodeUtility.GetCTLCode(DeviceType.FILE_DEVICE_UNKNOWN, 0x801, MethodType.METHOD_BUFFERED, AccessType.FILE_ANY_ACCESS);
        }

        public KernelTable CreateGDT(DeviceDataTransmitter deviceDataTransmitter, uint processorNumber)
        {
            byte[] data = deviceDataTransmitter.ExecDeviceIoControl(CTLCode, BitConverter.GetBytes(processorNumber));
            return GDT.Parse(processorNumber, data, 0);
        }
    }
}
