using System.ComponentModel;

namespace Mbhv
{
    public class ProgressObject : INotifyPropertyChanged
    {
        private double progressValue;
        public double ProgressValue
        {
            // ReSharper disable once UnusedMember.Global
            get => progressValue;
            set
            {
                progressValue = value;
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs("ProgressValue"));
            }
        }

        #region INotifyPropertyChanged Members
        public event PropertyChangedEventHandler PropertyChanged;
        #endregion
    }
}
