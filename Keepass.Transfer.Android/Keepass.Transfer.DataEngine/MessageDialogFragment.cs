using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;

namespace Keepass.Transfer.DataEngine
{
    public abstract class MessageDialogFragment : DialogFragment
    {
        public new const string Tag = "ResultDialogFragmentTag";
        private const string DialogTitle = "DIALOG_TITLE";
        private const string DialogText = "DIALOG_TEXT";

        public abstract EventHandler<DialogClickEventArgs> DialogReadyHandler { get; }

        protected MessageDialogFragment()
        {
            Cancelable = false;
        }

        protected MessageDialogFragment(int title, int text) : this()
        {
            var args = new Bundle();
            args.PutInt(DialogTitle, title);
            args.PutInt(DialogText, text);
            Arguments = args;
        }

        public override Dialog OnCreateDialog(Bundle savedInstanceState)
        {
            int title = Arguments.GetInt(DialogTitle);
            int text = Arguments.GetInt(DialogText);

            var dialog = new AlertDialog.Builder(Activity)
                .SetTitle(title)
                .SetMessage(text)
                .SetCancelable(false)
                .SetPositiveButton(Android.Resource.String.Ok, DialogReadyHandler)
                .Create();
            dialog.SetCanceledOnTouchOutside(false);
            return dialog;
        }
    }
}