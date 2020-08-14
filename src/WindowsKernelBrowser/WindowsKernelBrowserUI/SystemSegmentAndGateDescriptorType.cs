using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    enum SystemSegmentAndGateDescriptorType : byte
    {
        LDT = 2,
        TSSAvailable = 9,
        TSSBusy = 11,
        CallGate = 12,
        InterruptGate = 14,
        TrapGate = 15
    }
}
