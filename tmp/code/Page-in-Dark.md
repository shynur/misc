```
./
 |__ (没有域)/
 |    |
 |    |__ extensions::service_worker
 |
 |__ hifhajfihheepannakeoceidiheallep/
      |
      |__ scripts/
           |
           |__ background.js
```

## `extensions::service_worker`

```javascript
(function(backgroundUrl, wakeEventPage, logging) {
  "use strict";
  self.chrome = self.chrome || {};
  self.chrome.runtime = self.chrome.runtime || {};
  function findBackgroundClient(){
      return self.clients
          .matchAll({includeUncontrolled : true, type : "window"})
          .then((function(clients){return clients.find((function(client){
              return client.url == backgroundUrl}))}))} function
      makeWakeEventPagePromise(){return new Promise((function(
          resolve, reject){wakeEventPage((function(success) {
        if (success)
          resolve();
        else
          reject('Failed to start background client "' + backgroundUrl + '"')
      }))}))} self.chrome.runtime.getBackgroundClient = function() {
    return findBackgroundClient().then((function(client) {
      if (client) {
        return client
      }
      return makeWakeEventPagePromise()
          .then((function(){return findBackgroundClient()}))
          .then((function(client) {
            if (!client) {
              return Promise.reject('Background client "' + backgroundUrl +
                                    '" not found')
            }
            return client
          }))
    }))
  }
});
```

## `background.js`

```javascript
// ++++++++++++++++++++++++++++++++
// + Written by Walter Choy       +
// + Copyright (c) 2021           +
// + Update Date: 10/9/2021       +
// ++++++++++++++++++++++++++++++++

// on window focus changed listener
chrome.windows.onFocusChanged.addListener(function(windowID) {
  sendUpdateMsg();
});

// on tab activated listener
chrome.tabs.onActivated.addListener(function(activeInfo) { sendUpdateMsg(); });

// send update message to content script
function sendUpdateMsg() {
  chrome.tabs.query(
      {active : true, currentWindow : true}, function(tabs) {
        if (chrome.runtime.lastError) {
          console.log(chrome.runtime.lastError.message);
        }
        if (tabs[0] != = undefined && tabs[0] != = null)
          chrome.tabs.sendMessage(
              tabs[0].id, {updateDarkMode : true}, function(response) {
                if (chrome.runtime.lastError) {
                  console.log(chrome.runtime.lastError.message);
                }
              });
      });
}
```
