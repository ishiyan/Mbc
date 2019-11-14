using System;
using System.Windows;
using System.Diagnostics;
using System.Windows.Controls;

namespace Mbhv
{
    /// <summary>
    /// Interaction logic for TraceTextBox.xaml
    /// </summary>
    // ReSharper disable once RedundantExtendsListEntry
    // ReSharper disable once ClassWithVirtualMembersNeverInherited.Global
    public partial class TraceTextBoxControl : UserControl, ITraceTextSink, IDisposable
    {
        private TraceListener listener;

        public TraceTextBoxControl()
        {
            if (!Properties.Settings.Default.TraceLogVisible)
            {
                AutoAttach = false;
                Visibility = Visibility.Collapsed;
            }
            else
                AutoAttach = true;
            InitializeComponent();
        }

        // ReSharper disable once UnusedMember.Global
        public bool HasText => textBox1.LineCount > 0;

        // ReSharper disable once MemberCanBePrivate.Global
        public bool Suppressed { get; set; }

        // ReSharper disable once MemberCanBePrivate.Global
        // ReSharper disable once AutoPropertyCanBeMadeGetOnly.Global
        public bool AutoAttach { get; set; }

        public void Event(string msg, TraceEventType eventType)
        {
            if (Suppressed)
                return;
            Dispatcher?.BeginInvoke((System.Threading.ThreadStart) delegate
            {
                textBox1.AppendText(msg); textBox1.ScrollToEnd();
            });
        }

        public void Fail(string msg)
        {
            if (Suppressed)
                return;
            Dispatcher?.BeginInvoke((System.Threading.ThreadStart) delegate
            {
                textBox1.AppendText(msg); textBox1.ScrollToEnd();
            });
        }

        private void UserControlLoaded(object sender, RoutedEventArgs e)
        {
            if (AutoAttach && listener == null)
            {
                listener = new TraceTextSource(this);
                Trace.Listeners.Add(listener);
                Debug.Listeners.Add(listener);
            }
        }

        private void UserControlUnloaded(object sender, RoutedEventArgs e)
        {
            if (listener != null)
            {
                Trace.Listeners.Remove(listener);
                listener.Dispose();
                listener = null;
            }
        }

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Usage", "CA2213:DisposableFieldsShouldBeDisposed", MessageId = "listener")]
        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                listener?.Dispose();
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
