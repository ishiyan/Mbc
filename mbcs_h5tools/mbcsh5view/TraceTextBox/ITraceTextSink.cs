using System.Diagnostics;

namespace Mbhv
{
    interface ITraceTextSink
    {
        void Fail(string msg);
        void Event(string msg, TraceEventType eventType);
    }
}
