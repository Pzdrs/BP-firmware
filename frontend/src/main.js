import {createApp} from 'vue'
import 'bootstrap/dist/css/bootstrap.css'
import App from './App.vue'

import {OhVueIcon, addIcons} from "oh-vue-icons";
import {
    MdWifi, MdWifi1Bar, MdWifi2Bar, MdWifioff
} from "oh-vue-icons/icons/md";

addIcons(
    MdWifi, MdWifi1Bar, MdWifi2Bar, MdWifioff
);

const app = createApp(App)

app.component('Icon', OhVueIcon)

app.mount('#app');
