using System;
using System.Collections.Generic;


namespace STEE.ISCS.MVC
{
    /// <summary>
    /// Test Class for Interface IViewFactory
    /// </summary>
    class ViewFactoryTest:IViewFactory
    {
        /// <summary>
        /// Creates and returns ViewTest view for type ViewTest.ViewTestType 
        /// otherwise return null
        /// </summary>
        /// <param name="type"></param>
        /// <param name="id"></param>
        /// <returns></returns>
        public IView CreateView(String type, String id)
        {
            IView view = null;
            IController controller = null;
            IModel model = null;
            switch (type)
            {
                case ViewTest.ViewTestType:
                    view = new ViewTest();
                    view.ViewType = ViewTest.ViewTestType;
                    controller = new ControllerTest();
                    model = new ModelTest();
                    controller.Attach(model, view);
                    break;
                default:
                    break;
            }
            return view;
        }
    }
}
