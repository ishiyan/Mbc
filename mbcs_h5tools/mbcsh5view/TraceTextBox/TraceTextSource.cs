using System.Diagnostics;

namespace Mbhv
{
    class TraceTextSource : TraceListener
    {
        private bool fail;

        // ReSharper disable once MemberCanBePrivate.Global
        public ITraceTextSink Sink { get; }

        private TraceEventType eventType = TraceEventType.Information;

        public TraceTextSource(ITraceTextSink sink)
        {
            Debug.Assert(sink != null);
            Sink = sink;
        }

        public override void Fail(string message)
        {
            fail = true;
            base.Fail(message);
        }

        public override void TraceEvent(TraceEventCache eventCache, string source, TraceEventType traceEventType, int id, string message)
        {
            eventType = traceEventType;
            base.TraceEvent(eventCache, source, traceEventType, id, message);
        }

        public override void Write(string message)
        {
            if (IndentLevel > 0)
                message = message?.PadLeft(IndentLevel + message.Length, '\t');

            if (fail)
                Sink.Fail(message);

            else
                Sink.Event(message, eventType);

            fail = false;
            eventType = TraceEventType.Information;
        }

        public override void WriteLine(string message)
        {
            Write(message + "\n");
        }
    }
}
