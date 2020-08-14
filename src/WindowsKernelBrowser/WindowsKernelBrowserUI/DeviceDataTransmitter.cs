using PInvoke;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class DeviceDataTransmitter
    {
        Device Device;
        public DeviceDataTransmitter()
        {
            Device = Device.NewOpenDevice("KernelBrowser");
        }
        public byte[] ExecDeviceIoControl(int IOCTLCode, byte[] inBuffer)
        {
            bool needAgainQuery = false;
            byte[] outBuffer = new byte[4096];
            do
            {
                bool isSuccess = Device.DeviceIoControl(IOCTLCode, inBuffer, outBuffer, out int returnLength);
                if (isSuccess)
                {
                    if (returnLength >= 5)
                    {
                        if (BitConverter.ToBoolean(outBuffer, 0))
                        {
                            outBuffer = outBuffer.Skip(1).Take(returnLength - 1).ToArray();
                            needAgainQuery = false;
                        }
                        else
                        {
                            outBuffer = new byte[BitConverter.ToUInt32(outBuffer, 1)];
                            needAgainQuery = true;
                        }
                    }
                    else
                    {
                        throw new Exception("从驱动中读取到错误的数据格式");
                    }
                }
                else
                {
                    var errorcode = Kernel32.GetLastError();
                    throw new Win32Exception(errorcode, $"IOCTL失败,win32错误码：{errorcode}");
                }
            } while (needAgainQuery);
            return outBuffer;
        }
        public void Close()
        {
            Device.Close();
        }
    }
}
