
/**
 * Create By Name extension
 * @author Anders Riggelsen
 */
 
/* Ported to HTML5 by Matt Esch */

(function (w) {

    var ACT_CREATEOBJ_AT_POS = 0,
        ACT_CREATEOBJ_AT_XY = 1,
        ACT_CREATEBKD_AT_POS = 2,
        ACT_CREATEBKD_AT_XY = 3,
        EXP_GETNAMEFROMFIXED = 0;    
    
    function CRunCreateByName() 
    {
    }

    function action(num, act)
    {
        var rh = this.rh,
            name, x, y, layer, t, position;
        
        switch (num) {
            case ACT_CREATEOBJ_AT_POS:
                name = act.getParamExpString(rh, 0);
                position = act.getParamPosition(rh, 1);
                layer = act.getParamExpression(rh, 2);
                createObject.call(this, name, position.x, position.y, layer);
                break;
            case ACT_CREATEOBJ_AT_XY:
                name = act.getParamExpString(rh, 0);
                x = act.getParamExpression(rh, 1);
                y = act.getParamExpression(rh, 2);
                layer = act.getParamExpression(rh, 3);
                createObject.call(this, name, x, y, layer);
                break;
            case ACT_CREATEBKD_AT_POS:
                name = act.getParamExpString(rh, 0);
                position = act.getParamPosition(rh, 1);
                t = act.getParamExpression(rh, 2);
                layer = act.getParamExpression(rh, 3);
                createBackdrop.call(this, name, position.x, position.y, t, layer);
                break;
            case ACT_CREATEBKD_AT_XY:
                name = act.getParamExpString(rh, 0);
                x = act.getParamExpression(rh, 1);
                y = act.getParamExpression(rh, 2);
                t = act.getParamExpression(rh, 3);
                layer = act.getParamExpression(rh, 4);
                createBackdrop.call(this, name, x, y, t, layer);
                break;
        }
    }

    // Expressions
    // -------------------------------------------------
    function expression(num) {
        var fixed, obj;
        if (num === EXP_GETNAMEFROMFIXED) {
            fixed = this.ho.getExpParam().getInt();
            obj = this.ho.getObjectFromFixed(fixed);
            if (obj && obj.hoOiList) {
                return obj.hoOiList.oilName;
            }
        }
    }
    
    function createObject(objName, x, y, layer) {
        var creationOi = -1,
            rh = this.rh,
            i, info, number, pHo;
            
        for(i = 0; i < rh.rhMaxOI; i += 1) {
            info = rh.rhOiList[i];
            if (info.oilName === objName) {
                creationOi = info.oilOi;
                break;
            }
        }
        
        if (creationOi === -1) {
            return;
        }

        if (layer >= rh.rhFrame.nLayers) {
            layer = rh.rhFrame.nLayers-1;
        }
        
        if(layer < -1) {
            layer = -1;
        }

        number = this.rh.f_CreateObject(rh.rhMaxOI, creationOi, x, y, 0, 0, layer, -1);
        
        if (number >= 0) {
            pHo = rh.rhObjectList[number];
            rh.rhEvtProg.evt_AddCurrentObject(pHo);
        }
    }

    function createBackdrop(objName, x, y, t, layer) {
        var rh = this.rh,
            frame = rh.rhFrame,
            i, clayer, j, plo, info, backdrop, imageHandle, image;

        // Find backdrop
        for (i = 0; i < frame.nLayers; i += 1) {
            clayer = frame.layers[i];
            for (j = 0; j < clayer.nBkdLOs; j += 1) {
                plo =  frame.LOList.getLOFromIndex(clayer.nFirstLOIndex + j);
                info = rh.rhApp.OIList.getOIFromHandle(plo.loOiHandle);
                if (info && info.oiName == objName && info.oiOC) {
                    image = rh.rhApp.imageBank.getImageFromHandle(info.oiOC.ocImage);
                    rh.addBackdrop(image, x, y, layer, t);                        
                }
            }
        }
    }
    
    CRunCreateByName.prototype = w.CServices.extend(new w.CRunExtension(),
    {
        action: action,
        expression: expression
    });
    
    w['CRunCreateByName'] = CRunCreateByName;
}(window))    
