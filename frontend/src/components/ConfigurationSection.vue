<script setup>
defineProps({
  subsections: {
    type: Array,
    required: true
  }
});
</script>

<template>
  <div class="accordion" id="connectivityAccordion">
    <div class="accordion-item" v-for="subsection in subsections" :key="subsection.id">
      <h2 class="accordion-header">
        <button class="accordion-button" :class="!subsection.open?'collapsed':''" type="button"
                data-bs-toggle="collapse"
                :data-bs-target="`#${subsection.id}-collapse`"
                aria-expanded="false" :aria-controls="`#${subsection.id}-collapse`">
          {{ subsection.label }}
        </button>
      </h2>
      <div :id="`${subsection.id}-collapse`" class="accordion-collapse collapse" :class="subsection.open?'show':''"
           data-bs-parent="#connectivityAccordion">
        <div class="accordion-body">
          <Component :is="subsection.component"/>
        </div>
      </div>
    </div>
  </div>
</template>

<style scoped>

</style>
