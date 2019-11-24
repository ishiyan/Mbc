using System.ComponentModel;

namespace Mbcsh5view
{
    public class ProgressObject : INotifyPropertyChanged
    {
        private double progressValue;
        public double ProgressValue
        {
            get => progressValue;
            set
            {
                progressValue = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("ProgressValue"));
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;
    }
}
