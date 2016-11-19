using Android.App;
using Android.OS;

namespace Keepass.Transfer.DataEngine
{
    internal sealed class ProgressDialogFragment : DialogFragment
    {
        public new const string Tag = "ProgressDialogFragmentTag";
        private const string TextKey = "TEXT_ID_KEY";
        private int _textId = Resource.String.progress_text_encrypt;

        public int TextId
        {
            get { return _textId; }
            set {
                _textId = value;
                (Dialog as ProgressDialog)?.SetMessage(GetString(_textId));
            }
        }

        public ProgressDialogFragment()
        {
            Cancelable = false;
        }

        public override Dialog OnCreateDialog(Bundle savedInstanceState)
        {
            _textId = savedInstanceState?.GetInt(TextKey, _textId) ?? _textId;
            var dialog = new ProgressDialog(Activity);
            dialog.SetTitle(Resource.String.progress_title);
            dialog.SetMessage(GetString(_textId));
            dialog.SetCancelable(false);
            dialog.SetCanceledOnTouchOutside(false);
            return dialog;
        }

        public override void OnSaveInstanceState(Bundle outState)
        {
            base.OnSaveInstanceState(outState);
            outState.PutInt(TextKey, _textId);
        }
    }
}