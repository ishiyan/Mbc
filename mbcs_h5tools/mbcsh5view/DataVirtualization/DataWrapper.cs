using System.ComponentModel;
using System.Diagnostics;

// ReSharper disable once CheckNamespace
namespace DataVirtualization
{
    public class DataWrapper<T> : INotifyPropertyChanged
    {
        private T data;

        public event PropertyChangedEventHandler PropertyChanged;

        public DataWrapper(int index)
        {
            Index = index;
        }

        public int Index { get; }

        // ReSharper disable once UnusedMember.Global
        public int ItemNumber => Index + 1;

        // ReSharper disable once UnusedMember.Global
        public bool IsLoading => data == null;

        public T Data
        {
            get => data;
            internal set
            {
                data = value;
                OnPropertyChanged("Data");
                OnPropertyChanged("IsLoading");
            }
        }

        public bool IsInUse => PropertyChanged != null;

        private void OnPropertyChanged(string propertyName)
        {
            Debug.Assert(GetType().GetProperty(propertyName) != null);
            var handler = PropertyChanged;
            handler?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
