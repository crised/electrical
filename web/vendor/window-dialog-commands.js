/**
 * @license Angular-E2E-Window-Dialog-Commands (https://github.com/katranci/Angular-E2E-Window-Dialog-Commands)
 * (c) 2013 Ahmet KATRANCI
 * License: MIT
 */

'use strict';


var isCommonJS = typeof window == "undefined" && typeof exports == "object";
var alertThrows = function ()
{
    window.alert = function (msg)
    {
        throw new Error(msg);
    };
};
var confirmThrows = function ()
{
    window.confirm = function (msg)
    {
        throw new Error(msg);
    };
};
var promptThrows = function ()
{
    window.prompt = function (msg)
    {
        throw new Error(msg);
    };
};
/**
 * Usage: alertOK() sets window.alert to return true when it is called in your application
 */
jasmine.Env.prototype.alertOK = function (expectedMessage)
{
    window.alert = function (msg)
    {
        if (undefined != expectedMessage && msg != expectedMessage) {
            throw new Error("Expected '" + expectedMessage + "' but got '" + msg + "'");
        }
        return true;
    }
};
var alertOK = function (expectedMessage)
{
    return jasmine.getEnv().alertOK(expectedMessage);
};
if (isCommonJS) {
    exports.alertOK = alertOK;
}


/**
 * Usage: confirmOK() sets window.confirm to return true when it is called in your application
 */
jasmine.Env.prototype.confirmOK = function (expectedMessage)
{
    window.confirm = function (msg)
    {
        if (undefined != expectedMessage && msg != expectedMessage) {
            throw new Error("Expected '" + expectedMessage + "' but got '" + msg + "'");
        }
        return true;
    }
};
var confirmOK = function (expectedMessage)
{
    return jasmine.getEnv().confirmOK(expectedMessage);
};
if (isCommonJS) {
    exports.confirmOK = confirmOK;
}


/**
 * Usage: confirmCancel() sets window.confirm to return false when it is called in your application
 */
jasmine.Env.prototype.confirmCancel = function (expectedMessage)
{
    window.confirm = function (msg)
    {
        if (undefined != expectedMessage && msg != expectedMessage) {
            throw new Error("Expected '" + expectedMessage + "' but got '" + msg + "'");
        }
        return false;
    }
};
var confirmCancel = function (expectedMessage)
{
    return jasmine.getEnv().confirmCancel(expectedMessage);
};
if (isCommonJS) {
    exports.confirmCancel = confirmCancel;
}


/**
 * Usage: setPromptValue(value) sets window.prompt to return `value` when it is called in your application
 */
jasmine.Env.prototype.setPromptValue = function (value, expectedMessage)
{
    window.prompt = function (msg)
    {
        if (undefined != expectedMessage && msg != expectedMessage) {
            throw new Error("Expected '" + expectedMessage + "' but got '" + msg + "'");
        }
        return value;
    }
};
var setPromptValue = function (value, expectedMessage)
{
    return jasmine.getEnv().setPromptValue(value, expectedMessage);
};
if (isCommonJS) {
    exports.setPromptValue = setPromptValue;
}
