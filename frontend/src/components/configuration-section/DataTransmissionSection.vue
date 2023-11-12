<script setup>

import ConfigurationSection from "../ConfigurationSection.vue";
import MQTTSubsection from "./subsection/connectivity/MQTTSubsection.vue";
import Subsection from "../../utils/utils";

const subsections = [
  new Subsection('mqtt2', 'MQTT', MQTTSubsection)
];
</script>

<template>
  <ConfigurationSection :subsections="subsections"/>
</template>

<style scoped>

</style>
