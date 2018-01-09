using System;
using System.Collections.Generic;


namespace STEE.ISCS.MVC
{
    /// <summary>
    /// Test Class for interface IView.
    /// </summary>
    public class ViewTest:IView
    {
        public const string ViewTestType = "Testing";
        private String m_ViewID;
        private IController m_controller = null;

        public String ViewID
        {
            get { return m_ViewID; }
            set { m_ViewID = value; }
        }


        private String m_ViewType;

        public String ViewType
        {
            get { return m_ViewType; }
            set { m_ViewType = value; }
        }    

        public void PutObject(String objectName, Object obj)
        {
                    
        }
        //Destroy view
        public void DestroyView()
        {

        }

        //Hide view
        public void HideView()
        {

        }

        //Show View
        public void ShowView(bool showModal)
        {

        }

        //Attach the view to a controller        
        public void AttachController(IController controller)
        {
            m_controller = controller;
        }

        public IController getController()
        {
            return m_controller;
        }
    }
}
