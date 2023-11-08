import {createApp} from 'vue'
import './assets/css/main.css'
import 'bootstrap/dist/css/bootstrap.css'
import 'bootstrap/js/dist/tab.js'
import 'bootstrap/js/dist/collapse.js'
import 'bootstrap/js/dist/modal.js'
import App from './App.vue'

import {OhVueIcon, addIcons} from "oh-vue-icons";
import {
    MdComparearrows,
    MdSignalcellularalt, MdSignalcellularoff,
    MdWifi, MdWifi1Bar, MdWifi2Bar, MdWifioff
} from "oh-vue-icons/icons/md";
import {HiRefresh} from "oh-vue-icons/icons/hi";

addIcons(
    MdSignalcellularalt, MdSignalcellularoff,
    MdWifi, MdWifi1Bar, MdWifi2Bar, MdWifioff,
    MdComparearrows,
    HiRefresh
);

const app = createApp(App)

app.component('Icon', OhVueIcon)

app.mount('#app');
