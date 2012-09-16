package Extensions 
{
	import Actions.*;
	import Application.CRunFrame;
	import Banks.CImage;
    import Conditions.*;
	import Events.CEventProgram;
    import Expressions.*;
	import Frame.CLayer;
	import Frame.CLO;
    import Objects.CObject;
	import OI.COCBackground;
	import OI.COI;
	import Params.CPositionInfo;
	import Params.PARAM_OBJECT;
    import RunLoop.*;
    import Services.*;
    import Sprites.*;
	
	/**
	 * Create By Name extension
	 * @author Anders Riggelsen
	 */
	
	public class CRunCreateByName extends CRunExtension
	{
		
		private static var ACT_CREATEOBJ_AT_POS:int = 0;
		private static var ACT_CREATEOBJ_AT_XY:int = 1;
		private static var ACT_CREATEBKD_AT_POS:int = 2;
		private static var ACT_CREATEBKD_AT_XY:int = 3;
		private static var EXP_GETNAMEFROMFIXED:int = 0;
		
		public function CRunCreateByName() 
		{
		}
		
		public override function action(num:int, act:CActExtension):void
        {
			var name:String, x:int, y:int, layer:int, type:int, position:CPositionInfo;
			switch (num)
			{
				case ACT_CREATEOBJ_AT_POS:
				{
					name = act.getParamExpString(rh, 0);
					position = act.getParamPosition(rh, 1);
					layer = act.getParamExpression(rh, 2);
					createObject(name, position.x, position.y, layer);
					break;
				}
				case ACT_CREATEOBJ_AT_XY:
				{
					name = act.getParamExpString(rh, 0);
					x = act.getParamExpression(rh, 1);
					y = act.getParamExpression(rh, 2);
					layer = act.getParamExpression(rh, 3);
					createObject(name, x, y, layer);
					break;
				}
				case ACT_CREATEBKD_AT_POS:
				{
					name = act.getParamExpString(rh, 0);
					position = act.getParamPosition(rh, 1);
					type = act.getParamExpression(rh, 2);
					layer = act.getParamExpression(rh, 3);
					createBackdrop(name, position.x, position.y, type, layer);
					break;
				}
				case ACT_CREATEBKD_AT_XY:
				{
					name = act.getParamExpString(rh, 0);
					x = act.getParamExpression(rh, 1);
					y = act.getParamExpression(rh, 2);
					type = act.getParamExpression(rh, 3);
					layer = act.getParamExpression(rh, 4);
					createBackdrop(name, x, y, type, layer);
					break;
				}
			}
        }

        // Expressions
        // -------------------------------------------------
        public override function expression(num:int):CValue
        {
			var ret:CValue = new CValue(0);
			if(num == EXP_GETNAMEFROMFIXED)
			{
				var fixed:int = ho.getExpParam().getInt();
				var obj:CObject = ho.getObjectFromFixed(fixed);
				if (obj != null)
				{
					ret.forceString(obj.hoOiList.oilName);
					return ret;
				}
			}
			ret.forceString("");
			return ret;
        }
		
		public function createObject(objName:String, x:int, y:int, layer:int):void
		{
			var creationOi:int = -1;
			for(var i:int=0; i<rh.rhMaxOI; ++i)
			{
				var info:CObjInfo = rh.rhOiList[i];
				if(info.oilName == objName)
				{
					creationOi = info.oilOi;
					break;
				}
			}
			if(creationOi == -1)
				return;

			if(layer >= rh.rhFrame.nLayers)
				layer = rh.rhFrame.nLayers-1;
			if(layer < -1)
				layer = -1;

			var number:int=rh.f_CreateObject(rh.rhMaxOI, creationOi, x, y, 0, 0, layer, -1);
			if (number>=0)
			{
				var pHo:CObject=rh.rhObjectList[number];
				rh.rhEvtProg.evt_AddCurrentObject(pHo);
			}
		}

		public function createBackdrop(objName:String, x:int, y:int, type:int, layer:int):void
		{
			var frame:CRunFrame = rh.rhFrame;

			// Find backdrop
			for(var i:int=0; i<frame.nLayers; ++i)
			{
				var clayer:CLayer = frame.layers[i];
				for(var j:int=0; j<clayer.nBkdLOs; ++j)
				{
					var plo:CLO =  frame.LOList.getLOFromIndex(clayer.nFirstLOIndex + j);
					var info:COI = rh.rhApp.OIList.getOIFromHandle(plo.loOiHandle);
					if(info.oiName == objName)
					{
						var backdrop:COCBackground = COCBackground(info.oiOC);
						var imageHandle:int = backdrop.ocImage;
						var image:CImage = rh.rhApp.imageBank.getImageFromHandle(imageHandle);
						rh.addBackdrop(image, x, y, layer, type);						
					}
				}
			}
		}
		
	}

}