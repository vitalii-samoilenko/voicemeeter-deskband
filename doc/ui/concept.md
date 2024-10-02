# Scene

![Scene](scene.svg)

## Canvas

Render target for glyphs

- Resizable

### Glyph

Base render element

- Scalable

## Composition

Tree structured list of components

- Abstract
- Component as representative

### Component

Base UI element

- Scalable
- Interactive

#### Panel

Arranged group of components

- Passive

##### Stack panel

Arranges components starting from anchor side in non-overlapping way

![Stack panel](stack_panel.svg)

#### Control

Functional component

- Visible

##### State control

Maintains internal state and state change policy

![State control](state_control.svg)

## Scalability

![Scalability](scalability.svg)