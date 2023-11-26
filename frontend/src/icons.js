import {OhVueIcon, addIcons} from "oh-vue-icons";
import {
    MdComparearrows, MdRestartalt,
    MdSignalcellularalt, MdSignalcellularoff,
    MdWifi, MdWifi1Bar, MdWifi2Bar, MdWifioff
} from "oh-vue-icons/icons/md";
import {HiRefresh} from "oh-vue-icons/icons/hi";
import {GiSattelite} from "oh-vue-icons/icons/gi";

addIcons(
    MdSignalcellularalt, MdSignalcellularoff,
    MdWifi, MdWifi1Bar, MdWifi2Bar, MdWifioff,
    MdComparearrows,
    HiRefresh,
    GiSattelite,
    MdRestartalt
);

export default OhVueIcon;
