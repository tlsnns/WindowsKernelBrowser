using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    abstract class KernelTable
    {
        public uint ProcessorNumber { get; protected set; }
        public ulong TableAddress { get; protected set; }
        public int EntityCount { get; protected set; }
        public KernelTable(uint processorNumber)
        {
            ProcessorNumber = processorNumber;
        }
    }
}
