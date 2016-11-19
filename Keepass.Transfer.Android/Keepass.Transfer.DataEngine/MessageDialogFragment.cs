using System;
using Android.App;
using Android.Content;
using Android.OS;

namespace Keepass.Transfer.DataEngine
{
    public abstract class MessageDialogFragment : DialogFragment
    {
        public new const string Tag = "ResultDialogFragmentTag";
        private const string DialogTitle = "DIALOG_TITLE";
        private const string DialogTextId = "DIALOG_TEXT_ID";
        private const string DialogTextString = "DIALOG_TEXT_STRING";

        public abstract EventHandler<DialogClickEventArgs> DialogReadyHandler { get; }

        protected MessageDialogFragment()
        {
            Cancelable = false;
        }

        protected MessageDialogFragment(int title, int text) : this()
        {
            var args = new Bundle();
            args.PutInt(DialogTitle, title);
            args.PutInt(DialogTextId, text);
            Arguments = args;
        }

        protected MessageDialogFragment(int title, string text) : this()
        {
            var args = new Bundle();
            args.PutInt(DialogTitle, title);
            args.PutString(DialogTextString, text);
            Arguments = args;
        }

        public override Dialog OnCreateDialog(Bundle savedInstanceState)
        {
            int title = Arguments.GetInt(DialogTitle);
            int textId = Arguments.GetInt(DialogTextId, -1);
            string text = Arguments.GetString(DialogTextString, null);

            var builder = new AlertDialog.Builder(Activity)
                .SetTitle(title)
                .SetCancelable(false)
                .SetPositiveButton(Android.Resource.String.Ok, DialogReadyHandler);
            if (text != null)
                builder.SetMessage(text);
            else
                builder.SetMessage(textId);
            var dialog = builder.Create();
            dialog.SetCanceledOnTouchOutside(false);
            return dialog;
        }
    }
}