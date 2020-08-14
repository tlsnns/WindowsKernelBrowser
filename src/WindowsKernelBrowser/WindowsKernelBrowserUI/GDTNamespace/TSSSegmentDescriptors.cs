using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WindowsKernelBrowserUI
{
    class TSSSegmentDescriptors : GDTEntity
    {
        public SystemSegmentAndGateDescriptorType Type { get; private set; }
        TSSSegmentDescriptors(byte[] vs, int startIndex) : base(vs, startIndex)
        {
            var a = vs[startIndex + 13];//13-1
            var rw = vs[startIndex + 14];//14-1
            var ce = vs[startIndex + 15];//15-1
            var cd = vs[startIndex + 16];//16-1
            var r = (cd << 3) | (ce << 2) | (rw << 1) | a;
            Type = (SystemSegmentAndGateDescriptorType)r;
        }
        public static TSSSegmentDescriptors Parse(byte[] vs, int startIndex)
        {
            return new TSSSegmentDescriptors(vs, startIndex);
        }

        public override Dictionary<string, string> ToDictionaries()
        {
            var d = base.ToDictionaries();
            d.Add("Type", Type.ToString());
            return d;
        }
    }
}
