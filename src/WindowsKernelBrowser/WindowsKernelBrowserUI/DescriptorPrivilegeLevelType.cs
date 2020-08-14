using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    enum DescriptorPrivilegeLevelType : byte
    {
        Ring0 = 0,
        Ring1 = 1,
        Ring2 = 2,
        Ring3 = 3
    }
}
